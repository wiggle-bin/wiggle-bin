<script lang="ts" context="module">
	export async function getContoursData(startTime: string, endTime: string) {
		const response = await fetch(
			`http://127.0.0.1:8000/contours?startTime=${startTime}&endTime=${endTime}`
		);
		const data = await response.json();

		return data;
	}
    export function convertDates(dates: any) {
        return dates.map((date: string, index: number) => {
			const [year, month, dayAndTime] = date.split('-');
			const [day, time] = dayAndTime.split('_');
			const hour = time.slice(0, 2);
			const minute = time.slice(2, 4);
			return new Date(+year, +month, +day, +hour, +minute).getMinutes();
		})
    }
</script>
